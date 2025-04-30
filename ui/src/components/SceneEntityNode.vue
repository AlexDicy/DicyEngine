<script setup lang="ts">
import { computed, ref } from 'vue';

export interface EntityNode {
  id: number;
  name: string;
  type: string;
  children?: EntityNode[];
}

const props = defineProps<{
  node: EntityNode;
  selectedId: number;
  onSelect: (id: number) => void;
}>();

const hasChildren = computed(() => !!props.node.children?.length);
const showChildren = ref(true);
const isSelected = computed(() => props.selectedId === props.node.id);
</script>

<template>
  <div class="text-sm">
    <div class="flex items-center cursor-pointer" :class="{'bg-gray-800': isSelected}" @click="onSelect(props.node.id)">
      <span class="text-gray-400 material-symbols-rounded" :class="{invisible: !hasChildren}"
            @click="showChildren = !showChildren">
        {{ showChildren ? 'keyboard_arrow_down' : 'chevron_right' }}
      </span>
      <span class="text-gray-500 font-mono text-xs">{{ props.node.type.charAt(0) }}</span>
      <span class="text-gray-400">&nbsp;{{ props.node.name }}</span>
    </div>
    <div v-if="hasChildren && showChildren" class="ml-4">
      <SceneEntityNode v-for="child of props.node.children" :node="child" :selected-id="props.selectedId"
                       :on-select="onSelect" />
    </div>
  </div>
</template>
