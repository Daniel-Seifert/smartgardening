<template>
  <v-app>
    <v-app-bar app class="green--text text--darken-2" dark>
      <div class="d-flex align-center" @click="$router.push('/')">
        <v-img
          alt="Smart Gardening"
          class="shrink mr-2"
          contain
          src="./assets/min-logo.png"
          transition="scale-transition"
          width="45"
        />
        <h2>
          SmartGardening
        </h2>
      </div>
      <v-spacer></v-spacer>
    </v-app-bar>

    <v-main>
      <transition name="fade" mode="out-in">
        <v-container>
          <router-view></router-view>
        </v-container>
      </transition>
    </v-main>
  </v-app>
</template>

<script lang="ts">
import Vue from "vue";

export default Vue.extend({
  name: "App",

  data: () => ({
    //
  }),
});
</script>

<style>
.fade-enter-active,
.fade-leave-active {
  transition-duration: 0.3s;
  transition-property: opacity;
  transition-timing-function: ease;
}

.fade-enter,
.fade-leave-active {
  opacity: 0;
}
</style>
