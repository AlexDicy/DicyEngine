<script setup lang="ts">
import { ref } from 'vue';
import SceneEntityNode, { type EntityNode } from '@/components/SceneEntityNode.vue';

const tree = ref<EntityNode[]>([
  {
    id: Math.random(),
    name: 'Camera',
    type: 'Camera',
  },
  {
    id: Math.random(),
    name: 'Blue Point light',
    type: 'PointLight',
  },
  {
    id: Math.random(),
    name: 'Point light',
    type: 'PointLight',
  },
  {
    id: Math.random(),
    name: 'Rotating entity',
    type: 'Entity',
    children: [
      {
        id: Math.random(),
        name: 'Bench',
        type: 'Mesh',
      },
      {
        id: Math.random(),
        name: 'Ground',
        type: 'Mesh',
      },
      {
        id: Math.random(),
        name: 'Picnic wooden table',
        type: 'Mesh',
      },
    ],
  },
]);

const selectedId = ref(Math.random());
</script>

<template>
  <div class="window-contents">
    Scene entities
    <SceneEntityNode v-for="node in tree" :node="node" :selected-id="selectedId" :on-select="id => selectedId = id" />
  </div>
</template>
