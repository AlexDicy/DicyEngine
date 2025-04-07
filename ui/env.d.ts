/// <reference types="vite/client" />

declare global {
  interface Window {
    setMessageListener: (message: string, callback: Function) => void;
    call: (name: string, ...args: any[]) => Promise<any>;
    cefCall: (name: string, resolve: Function, reject: Function, ...args: any[]) => void;
  }
}

export {};
