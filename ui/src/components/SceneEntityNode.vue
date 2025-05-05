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
  depth: number;
}>();

const hasChildren = computed(() => !!props.node.children?.length);
const showChildren = ref(true);
const isSelected = computed(() => props.selectedId === props.node.id);
</script>

<template>
  <div class="text-sm">
    <div class="entity" :class="{active: isSelected}" :style="{ paddingLeft: `${props.depth}rem` }" @click="onSelect(props.node.id)">
      <div class="entity-expand-icon material-symbols-rounded" :class="{invisible: !hasChildren}"
            @click.stop="showChildren = !showChildren">
        {{ showChildren ? 'keyboard_arrow_down' : 'chevron_right' }}
      </div>
      <div class="entity-icon material-symbols-rounded">deployed_code</div>
<!--      <span class="entity-icon">{{ props.node.type.charAt(0) }}</span>-->
      <div class="entity-name">&nbsp;{{ props.node.name }}</div>
    </div>
    <div v-if="hasChildren && showChildren">
      <SceneEntityNode v-for="child of props.node.children" :node="child" :selected-id="props.selectedId"
                       :on-select="onSelect" :depth="props.depth + 1" :key="child.id" />
    </div>
  </div>
</template>

<style scoped>
@reference "@/assets/main.css";

.entity {
  @apply flex items-center cursor-pointer py-0.5;
}

.entity.active {
  @apply bg-gray-800;
}

.entity-expand-icon {
  @apply text-gray-400 text-xl/1;
}

.entity-icon {
  @apply text-gray-500 text-sm/1;
}

.entity-name {
  @apply text-gray-400 pb-0.5;
}

.active {
  & > .entity-expand-icon {
    @apply text-gray-300;
  }

  & > .entity-icon {
    @apply text-gray-400;
  }

  & > .entity-name {
    @apply text-gray-50;
  }
}
</style>
