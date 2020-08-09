<template>
  <v-expansion-panels v-model="selectedIndex" :popout="true">
    <v-layout row wrap>
      <v-flex xs12>
        <v-expansion-panel v-for="(device, i) in devices" :key="i">
          <v-expansion-panel-header v-slot="{ open }">
            <v-layout align-center justify-space-between row fill-height>
              <v-flex lg6 xs8>
                <v-badge
                  :color="device.activated ? 'green' : 'red'"
                  :content="device.activated ? 'active' : 'inactive'"
                >
                  <h2>{{ device.name }}</h2>
                </v-badge>
              </v-flex>
              <v-flex lg5 xs0>
                <v-fade-transition leave-absolute>
                  <span v-if="open" key="0">Aktuellste Messungen</span>
                </v-fade-transition>
              </v-flex>
              <v-flex lg1 xs4>
                <v-btn
                  :disabled="!device.activated"
                  class="mx-4"
                  fab
                  dark
                  outlined
                  color="green darken-2"
                  @click="openSetting(device.id)"
                >
                  <v-icon dark>settings</v-icon>
                </v-btn>
              </v-flex>
            </v-layout>
          </v-expansion-panel-header>

          <v-expansion-panel-content>
            <v-layout align-center justify-space-between row fill-height>
              <v-flex lg7 xs12>
                <line-chart
                  v-if="device.activated"
                  :height="200"
                  :chart-data="measurementDataCollection(device.id)"
                  :options="options"
                ></line-chart>
                <h2 v-else>Das Gerät muss für Messungen aktiviert werden</h2>
              </v-flex>
              <v-flex lg1 xs0>
                <v-divider vertical class="mx-4"></v-divider>
              </v-flex>
              <v-flex lg4 xs12>
                Informationen zum Gerät
                <v-list-item>
                  <v-list-item-content>
                    <v-list-item-title>UUID</v-list-item-title>
                    <v-list-item-subtitle>{{ device.id }}</v-list-item-subtitle>
                  </v-list-item-content>
                </v-list-item>
                <v-list-item v-if="device.activated">
                  <v-list-item-content>
                    <v-list-item-title>Feuchtigkeit</v-list-item-title>
                    <v-list-item-subtitle>
                      {{
                      deviceStatus(device.id).humidity
                      }}
                    </v-list-item-subtitle>
                  </v-list-item-content>
                </v-list-item>
                <v-list-item>
                  <v-list-item-content v-if="device.activated">
                    <v-list-item-title>Letzte Bewässerung</v-list-item-title>
                    <v-list-item-subtitle>
                      {{
                      deviceStatus(device.id).lastWatering
                      }}
                    </v-list-item-subtitle>
                  </v-list-item-content>
                </v-list-item>
              </v-flex>
              <v-flex lg12 xs10>
                <v-layout align-center justify-end row>
                  <v-flex lg2 xs4>
                    <v-btn
                      v-if="!device.activated"
                      text
                      color="success"
                      @click="activateDevices(device.id)"
                    >Aktivieren</v-btn>
                  </v-flex>
                  <v-flex lg2 xs4>
                    <v-btn text color="error" @click="deleteDevice(device.id)">Löschen</v-btn>
                  </v-flex>
                </v-layout>
              </v-flex>
            </v-layout>
          </v-expansion-panel-content>
        </v-expansion-panel>
      </v-flex>
    </v-layout>
  </v-expansion-panels>
</template>

<script lang="ts">
import Vue from "vue";
import { Device } from "../model/Device";
import LineChart from "./LineChart.vue";
import { mapGetters } from "vuex";
import { mapActions } from "vuex";

export default Vue.extend({
  name: "Devices",
  components: {
    LineChart
  },
  data: () => ({
    deletePressCount: 0,
    selectedIndex: -1,
    stats: {
      humidity: "57%",
      lastWatering: "14.07.2020 19:10"
    },
    datacollection: {},
    options: {
      responsive: true,
      maintainAspectRatio: true,
      height: 300,
      scales: {
        yAxes: [
          {
            display: true,
            ticks: {
              suggestedMin: 0,
              suggestedMax: 100
            }
          }
        ]
      }
    }
  }),
  mounted() {
    this.$store.dispatch("loadDevices");
  },
  computed: {
    ...mapGetters({
      devices: "allDevices",
      activatedDevices: "activatedDevices"
    }),
    ...mapGetters(["measurementDataCollection", "deviceStatus"])
  },
  watch: {
    selectedIndex: function() {
      if (this.selectedIndex !== undefined && this.selectedIndex !== -1) {
        this.loadMeasurements(this.devices[this.selectedIndex].id);
        this.loadStatus(this.devices[this.selectedIndex].id);
      }
    }
  },
  methods: {
    ...mapActions(["loadMeasurements", "loadStatus"]),
    openSetting(uuid: string) {
      this.$store.dispatch("loadDeviceConfig", uuid);
      this.$router.push(`devices/${uuid}/settings`);
    },

    activateDevices(uuid: string) {
      this.$store.dispatch("activateDevice", uuid);
    },
    deleteDevice(uuid: string) {
      this.deletePressCount += 1;
      setTimeout(() => {
        this.deletePressCount = 0;
      }, 5000);
      if (this.deletePressCount > 10) {
        this.$store.dispatch("deleteDevice", uuid);
        this.selectedIndex = -1;
        this.deletePressCount == 0;
      }
    }
  }
});
</script>
