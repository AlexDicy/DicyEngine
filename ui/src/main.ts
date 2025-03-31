import './assets/main.css'

import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

const app = createApp(App)

app.use(router)

window.setMessageListener ??= ((message, callback: Function) => {
	console.log('setMessageListener', message, callback);
});

app.mount('#app')
