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
  methods: {
    loadConfig: () => {
      return {
        outdoor: true,
        zipCode: "82256",
        minHumidity: 0.1,
        maxHumidity: 0.8,
        minWateringSeconds: 60,
        maxWateringSeconds: 360
      };
    }
  }
};
</script>
