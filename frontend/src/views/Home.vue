<template>
  <div>
    <Devices class="devices" />
  </div>
</template>

<script>
// @ is an alias to /src
import Devices from "@/components/Devices.vue";

export default {
  name: "Home",
  components: {
    Devices,
  },
};
</script>
