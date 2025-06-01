<script setup lang="ts">
import { ref } from 'vue';
import SceneEntityNode, { type EntityNode } from '@/components/SceneEntityNode.vue';
import useRepeatingCall from '@/composables/useRepeatingCall.ts';

const tree = ref<EntityNode[]>([]);
const selectedId = ref(-1);

useRepeatingCall(() => {
  window.call('getSceneTree').then((newTree: EntityNode[]) => {
    tree.value = newTree;
  });
}, 1000);

window.setMessageListener('updateSelectedEntity', (entityId: number) => {
  selectedId.value = entityId;
});

function onSelect(id: number) {
  selectedId.value = id;
  window.call('selectEntity', id);
}
</script>

<template>
  <div class="window-title">Scene entities</div>
  <div class="window-contents-no-margin">
    <SceneEntityNode v-for="node in tree" :node="node" :selected-id="selectedId" :on-select="onSelect" :depth="0" />
  </div>
</template>
