<script setup lang="ts">
import { computed, ref } from 'vue';

const deltaTime = ref(0);
const fps = computed(() => deltaTime.value ? (1.0 / deltaTime.value).toFixed(2) : 0);
const timings = ref<[string, number][]>([]);

window.setMessageListener('updateFrameInfo', (dT: number) => {
  deltaTime.value = dT;
});

window.setMessageListener('updateProfilingInfo', function(newTimings: [string, number][]) {
  timings.value = newTimings;
});

const message = ref('false');

function toggleVSync() {
  window.call('toggleVSync')
    .then(vsync => message.value = vsync)
    .catch(e => {
      message.value = 'Error ' + e;
    });
}

function formatFunctionSignature(name: string) {
  const split = name.split('::');
  if (split.length == 2) {
    const className = split[0].split(' ').pop();
    const functionName = split[1];
    return `<span class="text-gray-400">${className}</span>::<span class="text-gray-200">${functionName}</span>`;
  }
  return `<span class="text-gray-200">${name}</span>`;
}

</script>

<template>
  <div class="window-contents">
    <pre>FPS: {{ fps }}</pre>
    <button @click="toggleVSync" class="mt-2 px-2 py-1 bg-gray-700 hover:bg-gray-800 text-gray-50 rounded shadow">
      Toggle VSync
    </button>
    <pre class="text-sm mt-2">VSync: {{ message }}</pre>
    <div class="flex flex-col text-sm mt-4">
      <div v-for="[name, time] in timings" :key="name" class="flex flex-row justify-between gap-2">
        <pre v-html="formatFunctionSignature(name)"></pre>
        <pre class="w-1/6 text-right">{{ (time / 1000.0).toFixed(2) }} ms</pre>
      </div>
    </div>
  </div>
</template>
