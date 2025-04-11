<script setup lang="ts">
import { computed, onBeforeUnmount, ref } from 'vue';

const props = defineProps<{
  direction: 'vertical' | 'horizontal';
  resizerLocation: 'left' | 'right' | 'top' | 'bottom';
  defaultWidth?: number;
  minWidth?: number;
  defaultHeight?: number;
  minHeight?: number;
}>();

const { direction, resizerLocation, defaultWidth, minWidth, defaultHeight, minHeight } = props;
const size = ref(direction === 'horizontal' ? defaultWidth : defaultHeight);
const style = computed(() => ({
  width: size.value && direction === 'horizontal' ? size.value + 'px' : null,
  height: size.value && direction === 'vertical' ? size.value + 'px' : null,
  'flex-basis': size.value ? size.value + 'px' : null,
}));
const resizeInverted = computed(() => {
  if (direction === 'horizontal') {
    return resizerLocation === 'left';
  } else {
    return resizerLocation === 'top';
  }
});

const resizableRef = ref<HTMLElement>();

let currentSize = 0;
let startX = 0;
let startY = 0;

function onMouseMove(event: MouseEvent) {
  const oldPosition = direction === 'horizontal' ? startX : startY;
  const newPosition = direction === 'horizontal' ? event.pageX : event.pageY;
  const minSize = (direction === 'horizontal' ? minWidth : minHeight) ?? -1;
  const difference = resizeInverted.value ? oldPosition - newPosition : newPosition - oldPosition;
  const newSize = currentSize + difference;
  if (newSize > minSize) {
    size.value = newSize;
  }
}

function enableResizeHandler(event: MouseEvent) {
  currentSize = (direction === 'horizontal' ? resizableRef.value?.clientWidth : resizableRef.value?.clientHeight) ?? 0;
  startX = event.pageX;
  startY = event.pageY;
  document.addEventListener('mousemove', onMouseMove);
  document.addEventListener('mouseup', disableResizeHandler);
}

function disableResizeHandler() {
  document.removeEventListener('mousemove', onMouseMove);
}

onBeforeUnmount(disableResizeHandler);
</script>

<template>
  <div class="resizable" :style="style" ref="resizableRef">
    <slot />
    <div class="resizer-bar" :class="{[direction]: true, [resizerLocation]: true}" @mousedown="enableResizeHandler" />
  </div>
</template>

<style scoped>
.resizable {
  position: relative;
}

.resizer-bar {
  position: absolute;
  z-index: 10;
}

.resizer-bar:hover {
  background-color: rgba(255, 255, 255, 0.5);
}

.resizer-bar.horizontal {
  width: 6px;
  height: 100%;
  cursor: ew-resize;
}

.resizer-bar.vertical {
  width: 100%;
  height: 6px;
  cursor: ns-resize;
}

.resizer-bar.left {
  left: -3px;
}

.resizer-bar.right {
  right: -3px;
}

.resizer-bar.top {
  top: -3px;
}

.resizer-bar.bottom {
  bottom: -3px;
}
</style>
