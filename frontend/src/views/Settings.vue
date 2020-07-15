<template>
  <div class="home">
    <settings-card :config="loadConfig()" />
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
        minHumidity: 0.2,
        maxHumidity: 0.8,
        minWateringSeconds: 60,
        maxWateringSeconds: 360
      };
    }
  }
};
</script>
