/// <reference types="vite/client" />

declare global {
  interface Window {
    setMessageListener: (message: string, callback: Function) => void;
    sendMessage: (message: string, ...args: any[]) => void;
  }
}

export {};
