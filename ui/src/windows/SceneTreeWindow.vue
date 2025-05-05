<script setup lang="ts">
import { ref } from 'vue';
import SceneEntityNode, { type EntityNode } from '@/components/SceneEntityNode.vue';
import useRepeatingCall from '@/composables/useRepeatingCall.ts';

const tree = ref<EntityNode[]>([]);
const selectedId = ref(0);

useRepeatingCall(() => {
  window.call('getSceneTree').then((newTree: EntityNode[]) => {
    tree.value = newTree;
  });
}, 1000);
</script>

<template>
  <div class="window-title">Scene entities</div>
  <div class="window-contents">
    <SceneEntityNode v-for="node in tree" :node="node" :selected-id="selectedId" :on-select="id => selectedId = id" />
  </div>
</template>
