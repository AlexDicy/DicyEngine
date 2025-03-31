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
  <div class="m-2 p-4 bg-gray-100 rounded-lg shadow-md max-w-xs">
    <pre>FPS: {{ fps }}</pre>
  </div>
</template>
