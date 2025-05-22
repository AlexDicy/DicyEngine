<script setup lang="ts">
import ViewportWindow from '@/windows/ViewportWindow.vue';
import PerformanceWindow from '@/windows/PerformanceWindow.vue';
import Resizable from '@/components/Resizable.vue';
import SceneTreeWindow from '@/windows/SceneTreeWindow.vue';
import { ref, onMounted, onUnmounted } from 'vue';
const ctrlPressed = ref(false);
const shiftPressed = ref(false);
const altPressed = ref(false);
const metaPressed = ref(false);
const keyPressed = ref('');
const handleKeyDown = (event: KeyboardEvent) => {
  ctrlPressed.value = event.ctrlKey;
  shiftPressed.value = event.shiftKey;
  altPressed.value = event.altKey;
  metaPressed.value = event.metaKey;
  keyPressed.value = event.key;
};
const handleKeyUp = (event: KeyboardEvent) => {
  ctrlPressed.value = event.ctrlKey;
  shiftPressed.value = event.shiftKey;
  altPressed.value = event.altKey;
  metaPressed.value = event.metaKey;
  keyPressed.value = '';
};
onMounted(() => {
  window.addEventListener('keydown', handleKeyDown);
  window.addEventListener('keyup', handleKeyUp);
});
onUnmounted(() => {
  window.removeEventListener('keydown', handleKeyDown);
  window.removeEventListener('keyup', handleKeyUp);
});
</script>

<template>
  <div class="split-widget windows-grid">
    <Resizable class="column window" direction="horizontal" resizer-location="right" :defaultWidth="360"
               :minWidth="100">
      <div class="column-contents">
        <PerformanceWindow />
      </div>
    </Resizable>
    <div class="column content-main">
      <div class="column-contents">
        <Resizable direction="vertical" resizer-location="bottom" :defaultHeight="16" :minHeight="16"
                   class="window"></Resizable>
        <ViewportWindow class="content-main" />
        <Resizable direction="vertical" resizer-location="top" :defaultHeight="16" :minHeight="16"
                   class="window"></Resizable>
      </div>
    </div>
    <Resizable class="column window" direction="horizontal" resizer-location="left" :defaultWidth="200" :minWidth="100">
      <div class="column-contents">
        <Resizable class="column" direction="vertical" resizer-location="bottom" :defaultHeight="320" :minHeight="100">
          <div class="column-contents">
            <SceneTreeWindow />
          </div>
        </Resizable>
        <div class="column content-main">
          <div class="column-contents">
            <pre>Ctrl: {{ ctrlPressed }}</pre>
            <pre>Shift: {{ shiftPressed }}</pre>
            <pre>Alt: {{ altPressed }}</pre>
            <pre>Meta: {{ metaPressed }}</pre>
            <pre>Key: {{ keyPressed }}</pre>
            <input type="text" placeholder="type here..." />
          </div>
        </div>
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
