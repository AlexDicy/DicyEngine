import type { ClientInit } from '@sveltejs/kit';

export const init: ClientInit = () => {
	window.setMessageListener ??= ((message, callback: Function) => {
		console.log('setMessageListener', message, callback);
	});
};
