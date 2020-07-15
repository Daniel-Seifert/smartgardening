import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    devices: [
      {
        uuid: "550e8400-e29b-11d4-a716-446655440000",
        name: "First Device",
        activated: true
      },
      {
        uuid: "550e8400-e29b-11d4-a716-446655440001",
        name: "Second Device",
        activated: false
      },
      {
        uuid: "550e8400-e29b-11d4-a716-446655440002",
        name: "Third Device",
        activated: true
      }
    ],
    deviceConfig: [
      {
        uuid: "",
        config: null
      }
    ],
    measurementObjects: [
      {
        uuid: "550e8400-e29b-11d4-a716-446655440002",
        measurements: [1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12]
      }
    ]
  },
  mutations: {
    SET_DEVICES(state, devices) {
      state.devices.push(devices);
    },

    SET_DEVICECONFIG(state, config) {
      const index = state.deviceConfig.findIndex(c => c.uuid === config.uuid);
      if (index != -1) {
        state.deviceConfig[index].config = config.config;
      } else {
        state.deviceConfig.push(config);
      }
    },

    SET_MEASUREMENT(state, measurements) {
      const index = state.measurementObjects.findIndex(
        m => m.uuid === measurements.uuid
      );

      if (index != -1) {
        state.measurementObjects.push(measurements);
      } else {
        state.measurementObjects[index].measurements =
          measurements.measurements;
      }
    }
  },
  getters: {
    activatedDevices: state => {
      return state.devices.filter(device => device.activated);
    },
    allDevices: state => {
      return state.devices;
    },
    measurement: state => (uuid:string) => {
      return state.measurementObjects.find(
        measurement => measurement.uuid === uuid
      )?.measurements;
    }
  },
  actions: {
    loadDevices({ commit }) {
      axios.get("api/devices").then(({ data }) => {
        commit("SET_DEVICES", data);
      });
    },

    loadDevicesConfig({ commit }, uuid) {
      axios.get("api/devices/" + uuid + "/config").then(({ data }) => {
        commit("SET_DEVICECONFIG", { uuid: uuid, config: data });
      });
    },

    loadMeasurements({ commit, getters }, uuid) {
      axios.get(uuid + "/measurements").then(({ data }) => {
        commit("SET_MEASUREMENT", { uuid: uuid, measurements: data });
      });
    }
  }
});
