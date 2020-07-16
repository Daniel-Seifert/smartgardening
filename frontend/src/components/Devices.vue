<template>
  <v-expansion-panels v-model="selectedIndex" :popout="true">
    <v-expansion-panel v-for="(device, i) in devices" :key="i">
      <v-expansion-panel-header v-slot="{ open }">
        <v-row>
          <v-col cols="4">
            <v-badge
              :color="device.activated ? 'green' : 'red'"
              :content="device.activated ? 'active' : 'inactive'"
            >
              <h2>{{ device.name }}</h2>
            </v-badge>
          </v-col>
          <v-col cols="7" class="text--secondary">
            <v-fade-transition leave-absolute>
              <span v-if="open" key="0">
                Aktuellste Messungen
              </span>
            </v-fade-transition>
          </v-col>
          <v-col cols="1">
            <v-btn
              :disabled="!device.activated"
              class="mx-4"
              fab
              dark
              outlined
              color="green darken-2"
              @click="$router.push(`devices/${device.id}/settings`)"
            >
              <v-icon dark>settings</v-icon>
            </v-btn>
          </v-col>
        </v-row>
      </v-expansion-panel-header>

      <v-expansion-panel-content>
        <v-row no-gutters>
          <v-spacer></v-spacer>
          <v-col cols="5">
            <line-chart
              v-if="device.activated"
              :height="200"
              :chart-data="datacollection"
              :options="options"
            ></line-chart>
            <h2 v-else>Das Gerät muss für Messungen aktiviert werden</h2>
          </v-col>
          <v-spacer></v-spacer>
          <v-divider vertical class="mx-4"></v-divider>

          <v-col cols="3">
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
                <v-list-item-subtitle>{{
                  stats.humidity
                }}</v-list-item-subtitle>
              </v-list-item-content>
            </v-list-item>
            <v-list-item>
              <v-list-item-content v-if="device.activated">
                <v-list-item-title>Letzte Bewässerung</v-list-item-title>
                <v-list-item-subtitle>{{
                  stats.lastWatering
                }}</v-list-item-subtitle>
              </v-list-item-content>
            </v-list-item>
          </v-col>
        </v-row>

        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn v-if="!device.activated" text color="success" @click="activateDevices(device.name, device.id)">
            Aktivieren
          </v-btn>
          <v-btn text color="error">
            Löschen
          </v-btn>
        </v-card-actions>
      </v-expansion-panel-content>
    </v-expansion-panel>
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
  mounted(){
    this.$store.dispatch('loadDevices');
  },
  computed: {
    ...mapGetters({
      devices: "allDevices",
      activatedDevices: "activatedDevices"
    }),

    ...mapActions([
      "getMeassurements",
    ])
  },
  watch: {
    selectedIndex: function() {
      if (this.selectedIndex != -1) {
        this.loadStats();
        this.loadMeasures();
      }
    }
  },
  methods: {
    activateDevices(name, uuid) {
      this.$store.dispatch('activateDevice',{name: name, uuid: uuid});
    },
    loadStats() {
      // Reset stats
      this.stats = {
        humidity: "-",
        lastWatering: "-"
      };
      // Load stats async
      setTimeout(
        () =>
          (this.stats = {
            humidity: `57 %`,
            lastWatering: "14.07.2020 11:30"
          }),
        1000
      );
    },
    loadMeasures() {
      // Reset stats
      this.datacollection = {
        labels: [...Array(12).keys()].map(i => `${i}:00`),
        datasets: []
      };
      // Load stats async
      setTimeout(() => this.fillData(), 2000);
    },
    fillData() {
      this.datacollection = {
        labels: [...Array(12).keys()].map(i => `${i}:00`),
        datasets: [
          {
            label: "Feuchtigkeit in %",
            backgroundColor: "#1E88E5",
            data: [
              ...this.$store.getters.measurement(
                "550e8400-e29b-11d4-a716-446655440002"
              )
            ]
          }
        ]
      };
    },
    getRandomInt() {
      return Math.floor(Math.random() * (80 - 5 + 1)) + 5;
    }
  }
});
</script>
