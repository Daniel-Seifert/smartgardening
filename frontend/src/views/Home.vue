<style scoped>
.devices {
  padding-top: 20px;
  padding-left: 100px;
  padding-right: 100px;
  padding-bottom: 20px;
}
</style>

<template>
  <div class="home">
    <Devices class="devices" />
  </div>
</template>

<script>
// @ is an alias to /src
import Devices from "@/components/Devices.vue";

export default {
  name: "Home",
  components: {
    Devices
  }
};
</script>
