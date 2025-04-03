import './assets/main.css';

import { createApp } from 'vue';
import App from './App.vue';
import router from './router';

const app = createApp(App);

app.use(router);

window.setMessageListener ??= ((message, callback) => {
  console.log('Binding not found: setMessageListener', message, callback);
});

window.sendMessage ??= ((message, ...args) => {
  console.log('Binding not found: sendMessage', message, args);
});

app.mount('#app');
