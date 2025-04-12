<script setup lang="ts">
import { onBeforeUnmount, onMounted, ref } from 'vue';

const viewport = ref<HTMLElement>();

const resizeObserver = new ResizeObserver((entries) => {
  const rect = viewport.value!.getBoundingClientRect(); // ResizeObserver does not provide top/left coordinates
  const devicePixelContentBoxSize = entries[0].devicePixelContentBoxSize;
  const x = Math.floor(rect.x * window.devicePixelRatio);
  const y = Math.floor(rect.y * window.devicePixelRatio);
  const width = Math.ceil(devicePixelContentBoxSize[0].inlineSize);
  const height = Math.ceil(devicePixelContentBoxSize[0].blockSize);
  window.call('viewportResize', x, y, width, height);
});

onMounted(() => {
  resizeObserver.observe(viewport.value!);
});

onBeforeUnmount(() => {
  resizeObserver.disconnect();
});
</script>

<template>
  <div ref="viewport">
    <!-- empty, transparent div, to allow the viewport to be visible -->
  </div>
</template>
