<script setup lang="ts">
import ViewportWindow from '@/windows/ViewportWindow.vue';
import PerformanceWindow from '@/windows/PerformanceWindow.vue';
import Resizable from '@/components/Resizable.vue';
import ViewportWindow2 from '@/windows/ViewportWindow2.vue';
</script>

<template>
  <div class="split-widget windows-grid">
    <Resizable class="column" direction="horizontal" resizer-location="right" :defaultWidth="100" :minWidth="100">
      <div class="column-contents">
        <PerformanceWindow />
        <ViewportWindow2 />
        <ViewportWindow />
      </div>
    </Resizable>
    <div class="column content-main">
      <div class="column-contents">
        <Resizable direction="vertical" resizer-location="bottom" :defaultHeight="150" :minHeight="100" class="window">
          <PerformanceWindow />
        </Resizable>
        <ViewportWindow class="content-main" />
        <Resizable direction="vertical" resizer-location="top" :defaultHeight="150" :minHeight="100" class="window">
          <PerformanceWindow />
        </Resizable>
      </div>
    </div>
    <Resizable class="column" direction="horizontal" resizer-location="left" :defaultWidth="100" :minWidth="100">
      <div class="column-contents">
        <Resizable class="column" direction="vertical" resizer-location="bottom" :defaultHeight="200" :minHeight="100">
          <PerformanceWindow />
        </Resizable>
        <div class="column content-main">
          <ViewportWindow2 />
        </div>
        <Resizable class="column" direction="vertical" resizer-location="top" :defaultHeight="200" :minHeight="100">
          <PerformanceWindow />
        </Resizable>
      </div>
    </Resizable>
  </div>
</template>

<style scoped>
@reference "@/assets/main.css";

.windows-grid {
  display: flex;
  flex-direction: row;
  position: relative;
  height: 100%;
  overflow: hidden;
}

.column, .column-contents {
  display: flex;
  flex-direction: column;
  position: relative;
  contain: size style;
}

.column > * {
  overflow: auto;
}

.column-contents {
  flex: auto;
}

.content-main {
  flex: auto;
  min-width: 6rem;
  min-height: 6rem;
}

.split-widget {
  display: flex;
  overflow: hidden;
}

.window {
  @apply bg-gray-900 text-gray-50;
}
</style>
