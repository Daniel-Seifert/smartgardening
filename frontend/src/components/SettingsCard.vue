<template>
  <v-card class="mx-auto" :elevation="4" outlined>
    <v-container>
      <v-row align="center">
        <v-col cols="5" justify="space-around" style="padding-left: 50px">
          <v-text-field
            v-model="name"
            :counter="50"
            :rules="nameRules"
            label="Name"
            required
          ></v-text-field>
          <v-row>
            <v-switch v-model="outdoor" class="ma-4" label="Outdoor"></v-switch>
            <v-spacer></v-spacer>
            <v-fade-transition leave-absolute>
              <v-text-field
                v-if="outdoor"
                v-model="zipCode"
                :counter="10"
                :rules="zipCodeRules"
                label="Postleitzahl"
                shrink
                required
              ></v-text-field>
            </v-fade-transition>
          </v-row>
        </v-col>
        <v-spacer></v-spacer>
        <v-col cols="2">
          <v-btn color="success" class="mr-4" @click="save">
            Speichern
          </v-btn>
          <v-btn color="error" class="mr-4" @click="save">
            Löschen
          </v-btn>
        </v-col>
      </v-row>

      <v-row>
        <v-col col="4" style="padding-right: 50px; padding-left: 50px;">
          <h3>Minimale Bodenfeuchtigkeit für Bewässerung</h3>
          <v-slider
            style="margin-top: 30px"
            v-model="minHumidity"
            :thumb-size="30"
            thumb-label="always"
          >
            <template v-slot:thumb-label="{ value }"> {{ value }}% </template>
          </v-slider>

          <h3>Maximale Bodenfeuchtigkeit für Bewässerung</h3>
          <v-slider
            style="margin-top: 30px"
            v-model="maxHumidity"
            :thumb-size="30"
            thumb-label="always"
          >
            <template v-slot:thumb-label="{ value }"> {{ value }}% </template>
          </v-slider>
        </v-col>
        <v-col col="8" style="padding-left: 50px; padding-right: 50px;">
          <h3>Minimale Bewässerungsdauer</h3>
          <v-slider
            style="margin-top: 55px"
            v-model="minWatering"
            :thumb-size="50"
            max="3600"
            thumb-label="always"
          >
            <template v-slot:thumb-label="{ value }">
              {{ (value / 60).toFixed() }}m{{ (value % 60).toFixed() }}s
            </template>
          </v-slider>

          <h3>Maximale Bewässerungsdauer</h3>
          <v-slider
            style="margin-top: 55px"
            v-model="maxWatering"
            :thumb-size="50"
            max="3600"
            thumb-label="always"
          >
            <template v-slot:thumb-label="{ value }">
              {{ (value / 60).toFixed() }}m{{ (value % 60).toFixed() }}s
            </template>
          </v-slider>
        </v-col>
      </v-row>
    </v-container>
  </v-card>
</template>

<script lang="ts">
import Vue from "vue";
import { Device } from "../model/Device";
import { Config } from "../model/Config";
import Component from "vue-class-component";

export default Vue.extend({
  name: "Settings",
  props: {
    config: Object as () => Config
  },
  data: () => ({
    name: "",
    nameRules: [
      (v: any) => !!v || "Name wird benötigt",
      (v: any) =>
        (v && v.length <= 50) || "Name darf nicht mehr als 50 Zeichen haben"
    ],
    zipCode: "",
    zipCodeRules: [
      (v: any) => !!v || "Postleitzahl wird benötigt",
      (v: any) =>
        (v && v.length <= 10) ||
        "Postleitzahl darf nicht mehr als 10 Zeichen haben"
    ],
    outdoor: false,
    minHumidity: 0.2,
    maxHumidity: 0.8,
    minWatering: 60,
    maxWatering: 360
  }),
  methods: {
    save() {
      console.log("Saved");
    }
  }
});
</script>
