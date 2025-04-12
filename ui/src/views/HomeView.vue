<script setup lang="ts">
import ViewportWindow from '@/windows/ViewportWindow.vue';
import PerformanceWindow from '@/windows/PerformanceWindow.vue';
import Resizable from '@/components/Resizable.vue';
</script>

<template>
  <div class="split-widget windows-grid">
    <Resizable class="column window" direction="horizontal" resizer-location="right" :defaultWidth="200" :minWidth="100">
      <div class="column-contents">
        <PerformanceWindow />
      </div>
    </Resizable>
    <div class="column content-main">
      <div class="column-contents">
        <Resizable direction="vertical" resizer-location="bottom" :defaultHeight="80" :minHeight="80" class="window"></Resizable>
        <ViewportWindow class="content-main" />
        <Resizable direction="vertical" resizer-location="top" :defaultHeight="40" :minHeight="40" class="window"></Resizable>
      </div>
    </div>
    <Resizable class="column window" direction="horizontal" resizer-location="left" :defaultWidth="200" :minWidth="100">
      <div class="column-contents">
        <Resizable class="column" direction="vertical" resizer-location="bottom" :defaultHeight="200" :minHeight="100"></Resizable>
        <div class="column content-main"></div>
        <Resizable class="column" direction="vertical" resizer-location="top" :defaultHeight="200" :minHeight="100"></Resizable>
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
  @apply bg-gray-950 text-gray-50 border border-gray-900;
}
</style>
