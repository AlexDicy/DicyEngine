import './assets/main.css';

import { createApp } from 'vue';
import App from './App.vue';
import router from './router';

const app = createApp(App);

app.use(router);

window.setMessageListener ??= ((message, callback) => {
  console.log('Binding not found: setMessageListener', message, callback);
});

window.call = ((message, ...args) => {
  return new Promise((resolve, reject) => {
    window.cefCall(message, resolve, reject, args);
  });
});

window.cefCall ??= ((name, resolve, reject) => {
  console.log('Binding not found: cefCall', name);
  reject(new Error('Binding not found: cefCall'));
});

app.mount('#app');
