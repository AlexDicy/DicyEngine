import { onScopeDispose } from 'vue';

export default function useRepeatingCall(callback: () => void, waitTime: number) {
  let timer: number;

  function start() {
    try {
      callback();
    } catch (error) {
      console.error('Error in repeating call:', error);
    }
    // instead of setInterval to avoid overlapping calls
    timer = setTimeout(start, waitTime);
  }
  start();

  onScopeDispose(() => {
    clearTimeout(timer);
  });
}
