<script setup lang="ts">
import { computed, ref } from 'vue';

const deltaTime = ref(0);
const fps = computed(() => deltaTime.value ? (1.0 / deltaTime.value).toFixed(2) : 0);

window.setMessageListener('updateFrameInfo', (dT: number) => {
	console.log('deltaTime', dT);
	deltaTime.value = dT;
});
</script>

<template>
	<pre>deltaTime: {{ deltaTime }}</pre>
	<pre>FPS: {{ fps }}</pre>
</template>
