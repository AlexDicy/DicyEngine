<script setup lang="ts">
import { computed, onMounted, ref, shallowRef } from 'vue';
import { Chart, type ChartDataset } from 'chart.js/auto';

const deltaTime = ref(0);
const fps = computed(() => deltaTime.value ? (1.0 / deltaTime.value).toFixed(2) : 0);
const timings = ref<[string, number][]>([]);
const timingsHistory = ref<{ signature: string, timing: number }[][]>([]);
const MAX_TIMINGS = 100;

window.setMessageListener('updateFrameInfo', (dT: number) => {
  deltaTime.value = dT;
});

window.setMessageListener('updateProfilingInfo', function(newTimings: [string, number][]) {
  timings.value = newTimings.map(([signature, timing]) => {
    return [signature, timing / 1000.0];
  });
  timingsHistory.value.push(timings.value.map(([signature, timing]) => ({ signature, timing })));
  if (timingsHistory.value.length > MAX_TIMINGS) {
    timingsHistory.value.shift();
  }
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

const canvas = ref<HTMLCanvasElement>();
const chart = shallowRef<Chart | null>(null);

function getDatasetsForChart() {
  const timingsBySignature = new Map<string, number[]>();
  timingsHistory.value.forEach(frame => {
    for (const { signature, timing } of frame) {
      if (!timingsBySignature.has(signature)) {
        timingsBySignature.set(signature, []);
      }
      timingsBySignature.get(signature)!.push(timing);
    }
  });
  const datasets: ChartDataset[] = [];
  timingsBySignature.forEach((timings, signature) => {
    datasets.push({
      label: signature,
      data: timings,
      fill: true,
    });
  });
  return datasets;
}

function updateChart() {
  if (chart.value) {
    chart.value.data.datasets = getDatasetsForChart();
    chart.value.update();
    requestAnimationFrame(updateChart);
  }
}

onMounted(() => {
  chart.value = new Chart(canvas.value!, {
    type: 'line',
    data: {
      labels: Array.from({ length: MAX_TIMINGS }, (_, i) => i),
      datasets: getDatasetsForChart(),
    },
    options: {
      animation: false,
      scales: {
        x: {
          ticks: {
            display: false,
          },
          grid: {
            display: false,
          },
        },
        y: {
          stacked: true,
        },
      },
      plugins: {
        legend: {
          display: false,
        },
        tooltip: {
          mode: 'nearest',
          intersect: false,
        },
      },
    },
  });
  updateChart();
});
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
        <pre class="w-1/6 text-right">{{ time.toFixed(2) }} ms</pre>
      </div>
    </div>
    <canvas ref="canvas" class="mt-4" id="test"></canvas>
  </div>
</template>
