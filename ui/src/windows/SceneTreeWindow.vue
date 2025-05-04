<script setup lang="ts">
import { ref } from 'vue';
import SceneEntityNode, { type EntityNode } from '@/components/SceneEntityNode.vue';

const tree = ref<EntityNode[]>([]);
const selectedId = ref(0);

window.setMessageListener('updateSceneTree', (newTree: EntityNode[]) => {
  tree.value = newTree;
});

</script>

<template>
  <div class="window-contents">
    Scene entities
    <SceneEntityNode v-for="node in tree" :node="node" :selected-id="selectedId" :on-select="id => selectedId = id" />
  </div>
</template>
