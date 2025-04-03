<script setup lang="ts">
import { computed, ref } from 'vue';

const deltaTime = ref(0);
const fps = computed(() => deltaTime.value ? (1.0 / deltaTime.value).toFixed(2) : 0);

window.setMessageListener('updateFrameInfo', (dT: number) => {
  deltaTime.value = dT;
});

function toggleVSync() {
  window.sendMessage('toggleVSync');
}
</script>

<template>
  <div class="m-3 px-4 py-3 bg-gray-100 rounded-lg shadow-md max-w-xs">
    <pre>FPS: {{ fps }}</pre>
    <button @click="toggleVSync" class="mt-2 px-2 py-1 bg-gray-700 hover:bg-gray-800 text-gray-50 rounded shadow">
      Toggle VSync
    </button>
  </div>
</template>
