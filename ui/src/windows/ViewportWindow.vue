<script setup lang="ts">
import { onBeforeUnmount, onMounted, ref } from 'vue';

const viewport = ref<HTMLElement>();

const resizeObserver = new ResizeObserver((entries) => {
  const { width, height } = entries[0].contentRect;
  window.call('viewportResize', Math.ceil(width), Math.ceil(height));
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
