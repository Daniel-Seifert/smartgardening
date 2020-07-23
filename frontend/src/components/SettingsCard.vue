<template>
  <v-card :elevation="4" outlined style="width:100%">
    <v-container style="padding:50px">
      <v-layout align-start justify-space-around row wrap>
        <v-flex xs12>
          <v-layout align-start justify-space-around row wrap>
            <v-flex lg7 xs12 order-lg1 order-xs2>
              <v-text-field
                v-model="name"
                :counter="50"
                :rules="nameRules"
                label="Name"
                required
              ></v-text-field>
            </v-flex>
            <v-flex md2 xs5 order-lg2 order-xs1 offset-xs0 offset-lg2>
              <v-btn
                color="success"
                class="mr-4"
                @click="save($route.params.uuid)"
              >
                Speichern
              </v-btn>
            </v-flex>
          </v-layout>
        </v-flex>
        <v-flex xs12>
          <v-layout align-start row wrap>
            <v-flex lg2>
              <v-switch
                v-model="config.outdoor"
                class="ma-4"
                label="Outdoor"
              ></v-switch>
            </v-flex>
            <v-flex lg5>
              <v-fade-transition leave-absolute>
                <v-text-field
                  v-if="config.outdoor"
                  v-model="config.zipCode"
                  :counter="10"
                  :rules="zipCodeRules"
                  label="Postleitzahl"
                  shrink
                  required
                ></v-text-field>
              </v-fade-transition>
            </v-flex>
          </v-layout>
        </v-flex>
        <v-flex xs12>
          <v-layout align-start justify-space-around row wrap>
            <v-flex xs12 lg5 order-xs1 order-lg1>
              <h3>Minimale Bodenfeuchtigkeit für Bewässerung</h3>
              <v-slider
                style="margin-top: 30px"
                v-model="config.minHumidity"
                :thumb-size="30"
                thumb-label="always"
              >
                <template v-slot:thumb-label="{ value }">
                  {{ value }}%
                </template>
              </v-slider>
            </v-flex>

            <v-flex lg5 xs12 order-xs2 order-lg3>
              <h3>Maximale Bodenfeuchtigkeit für Bewässerung</h3>
              <v-slider
                style="margin-top: 30px"
                v-model="config.maxHumidity"
                :thumb-size="30"
                thumb-label="always"
              >
                <template v-slot:thumb-label="{ value }">
                  {{ value }}%
                </template>
              </v-slider>
            </v-flex>

            <v-flex lg5 xs12 order-xs3 order-lg2>
              <h3>Minimale Bewässerungsdauer</h3>
              <v-slider
                style="margin-top: 55px"
                v-model="config.minWateringSeconds"
                :thumb-size="50"
                max="3600"
                thumb-label="always"
                step="5"
              >
                <template v-slot:thumb-label="{ value }">
                  {{ Math.floor(value / 60) }}m {{ value % 60 }}s
                </template>
              </v-slider>
            </v-flex>

            <v-flex lg5 xs12 order-xs4 order-lg4>
              <h3>Maximale Bewässerungsdauer</h3>
              <v-slider
                style="margin-top: 55px"
                v-model="config.maxWateringSeconds"
                :thumb-size="50"
                max="3600"
                thumb-label="always"
                step="5"
              >
                <template v-slot:thumb-label="{ value }">
                  {{ Math.floor(value / 60) }}m {{ value % 60 }}s
                </template>
              </v-slider>
            </v-flex>
          </v-layout>
        </v-flex>
      </v-layout>
    </v-container>
  </v-card>
</template>

<script lang="ts">
import Vue from "vue";
import { Device } from "../model/Device";
import { Config } from "../model/Config";
import Component from "vue-class-component";

import { mapGetters } from "vuex";
import { mapActions } from "vuex";

export default Vue.extend({
  name: "Settings",
  props: ["initialName", "initialConfig"],

  created() {
    this.name = this.initialName;
    this.config = this.initialConfig;
  },

  computed: {
    ...mapGetters(["getDevice", "getDeviceConfig", "getDeviceHummidity"]),
  },

  data: () => ({
    name: "",
    config: {
      outdoor: false,
      zipCode: "",
      minHumidity: 0,
      maxHumidity: 0,
      minWateringSeconds: 0,
      maxWateringSeconds: 0,
    },
    nameRules: [
      (v: any) => !!v || "Name wird benötigt",
      (v: any) =>
        (v && v.length <= 50) || "Name darf nicht mehr als 50 Zeichen haben",
    ],
    zipCodeRules: [
      (v: any) => !!v || "Postleitzahl wird benötigt",
      (v: any) =>
        (v && v.length <= 10) ||
        "Postleitzahl darf nicht mehr als 10 Zeichen haben",
    ],
  }),
  methods: {
    save(uuid: string) {
      this.$store.dispatch("setDeviceConfig", {
        uuid: uuid,
        config: this.config,
      });
      this.$store.dispatch("setDeviceName", { uuid: uuid, name: this.name });
      this.$router.push("/");
    },
  },
});
</script>
