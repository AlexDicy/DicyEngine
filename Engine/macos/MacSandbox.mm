#include "pch/enginepch.h"

#import <Cocoa/Cocoa.h>

#include "include/cef_application_mac.h"
#include "Application.h"
#include "cef/OSRCefApp.h"
#include "cef/OSRCefHandler.h"

// Receives notifications from the application.
@interface SimpleAppDelegate : NSObject <NSApplicationDelegate>

- (void)createApplication:(id)object;
- (void)tryToTerminateApplication:(NSApplication*)app;
@end

// Provide the CefAppProtocol implementation required by CEF.
@interface SimpleApplication : NSApplication <CefAppProtocol> {
 @private
  BOOL handlingSendEvent_;
}
@end

@implementation SimpleApplication
- (BOOL)isHandlingSendEvent {
  return handlingSendEvent_;
}

- (void)setHandlingSendEvent:(BOOL)handlingSendEvent {
  handlingSendEvent_ = handlingSendEvent;
}

- (void)sendEvent:(NSEvent*)event {
  CefScopedSendingEvent sendingEventScoper;
  [super sendEvent:event];
}

// |-terminate:| is the entry point for orderly "quit" operations in Cocoa. This
// includes the application menu's quit menu item and keyboard equivalent, the
// application's dock icon menu's quit menu item, "quit" (not "force quit") in
// the Activity Monitor, and quits triggered by user logout and system restart
// and shutdown.
//
// The default |-terminate:| implementation ends the process by calling exit(),
// and thus never leaves the main run loop. This is unsuitable for Chromium
// since Chromium depends on leaving the main run loop to perform an orderly
// shutdown. We support the normal |-terminate:| interface by overriding the
// default implementation. Our implementation, which is very specific to the
// needs of Chromium, works by asking the application delegate to terminate
// using its |-tryToTerminateApplication:| method.
//
// |-tryToTerminateApplication:| differs from the standard
// |-applicationShouldTerminate:| in that no special event loop is run in the
// case that immediate termination is not possible (e.g., if dialog boxes
// allowing the user to cancel have to be shown). Instead, this method tries to
// close all browsers by calling CloseBrowser(false) via
// ClientHandler::CloseAllBrowsers. Calling CloseBrowser will result in a call
// to ClientHandler::DoClose and execution of |-performClose:| on the NSWindow.
// DoClose sets a flag that is used to differentiate between new close events
// (e.g., user clicked the window close button) and in-progress close events
// (e.g., user approved the close window dialog). The NSWindowDelegate
// |-windowShouldClose:| method checks this flag and either calls
// CloseBrowser(false) in the case of a new close event or destructs the
// NSWindow in the case of an in-progress close event.
// ClientHandler::OnBeforeClose will be called after the CEF NSView hosted in
// the NSWindow is dealloc'ed.
//
// After the final browser window has closed ClientHandler::OnBeforeClose will
// begin actual tear-down of the application by calling CefQuitMessageLoop.
// This ends the NSApplication event loop and execution then returns to the
// main() function for cleanup before application termination.
//
// The standard |-applicationShouldTerminate:| is not supported, and code paths
// leading to it must be redirected.
- (void)terminate:(id)sender {
  SimpleAppDelegate* delegate =
      static_cast<SimpleAppDelegate*>([NSApp delegate]);
  [delegate tryToTerminateApplication:self];
  // Return, don't exit. The application is responsible for exiting on its own.
}
@end

@implementation SimpleAppDelegate

// Create the application on the UI thread.
- (void)createApplication:(id)object {
  [[NSBundle mainBundle] loadNibNamed:@"MainMenu"
                                owner:NSApp
                      topLevelObjects:nil];

  // Set the delegate for application events.
  [[NSApplication sharedApplication] setDelegate:self];
}

- (void)tryToTerminateApplication:(NSApplication*)app {
  OSRCefHandler* handler = OSRCefHandler::getInstance();
  if (handler && !handler->isClosing()) {
    handler->closeAllBrowsers(false);
  }
}

- (NSApplicationTerminateReply)applicationShouldTerminate:
    (NSApplication*)sender {
  return NSTerminateNow;
}

// Called when the user clicks the app dock icon while the application is
// already running.
- (BOOL)applicationShouldHandleReopen:(NSApplication*)theApplication
                    hasVisibleWindows:(BOOL)flag {
  OSRCefHandler* handler = OSRCefHandler::getInstance();
  if (handler && !handler->isClosing()) {
    handler->showMainWindow();
  }
  return NO;
}

// Requests that any state restoration archive be created with secure encoding
// (macOS 12+ only). See https://crrev.com/c737387656 for details. This also
// fixes an issue with macOS default behavior incorrectly restoring windows
// after hard reset (holding down the power button).
- (BOOL)applicationSupportsSecureRestorableState:(NSApplication*)app {
  return YES;
}
@end

// Entry point function for the browser process.
int main(int argc, char* argv[]) {
  @autoreleasepool {
    // Initialize the SimpleApplication instance.
    [SimpleApplication sharedApplication];

    // If there was an invocation to NSApp prior to this method, then the NSApp
    // will not be a SimpleApplication, but will instead be an NSApplication.
    // This is undesirable and we must enforce that this doesn't happen.
    CHECK([NSApp isKindOfClass:[SimpleApplication class]]);

    // Create the application delegate.
    SimpleAppDelegate* delegate = [[SimpleAppDelegate alloc] init];
    // Set as the delegate for application events.
    NSApp.delegate = delegate;

    [delegate performSelectorOnMainThread:@selector(createApplication:)
                               withObject:nil
                            waitUntilDone:NO];


    const auto app = std::make_shared<Application>();
    app->initialize();
    app->run();

    // Release the delegate.
#if !__has_feature(objc_arc)
    [delegate release];
#endif  // !__has_feature(objc_arc)
    delegate = nil;
  }  // @autoreleasepool

  return 0;
}
