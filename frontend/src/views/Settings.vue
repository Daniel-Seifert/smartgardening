<template>
  <div class="home">
    <settings-card v-if="$store.getters.getDeviceConfig($route.params.uuid) !== undefined" :initialName="$store.getters.getDevice($route.params.uuid).name" :initialConfig="$store.getters.getDeviceConfig($route.params.uuid)" />
    {{$store.getters.getDeviceConfig($route.params.uuid)}}
  </div>
</template>

<script>
import SettingsCard from "@/components/SettingsCard.vue";
import { Config } from "../model/Config";

export default {
  name: "Settings",
  components: {
    SettingsCard
  },
};
</script>
