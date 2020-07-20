import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";
import { Device } from "../model/Device";
import { baseUrl } from "./baseUrl";
import moment from "moment";

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    devices: [] as Device[],
  },
  getters: {
    activatedDevices: (state) => {
      return state.devices.filter((device) => device.activated);
    },
    allDevices: (state) => {
      return state.devices;
    },
    getDevice: (state) => (uuid: string) => {
      return state.devices.find((device) => device.id === uuid);
    },
    getDeviceConfig: (state) => (uuid: string) => {
      return state.devices.find((device) => device.id === uuid)?.config;
    },
    getDeviceHummidity: (state) => (uuid: string) => {
      return state.devices.find((device) => device.id === uuid)?.config
        ?.minHumidity;
    },
    measurementDataCollection: (state) => (uuid: string) => {
      const measurements = state.devices.find((device) => device.id === uuid)
        ?.measurements;

      const emptyMeasruements = {
        labels: [],
        datasets: [
          {
            label: "No Data",
            backgroundColor: "#1E88E5",
            data: [],
          },
        ],
      };

      return measurements == null
        ? emptyMeasruements
        : {
            labels: [
              ...measurements.map((measure) =>
                moment(measure.createDate).format("dd HH:mm")
              ),
            ],
            datasets: [
              {
                label: "Feuchtigkeit in %",
                backgroundColor: "#1E88E5",
                data: [...measurements.map((measure) => measure.value)],
              },
            ],
          };
    },
  },
  mutations: {
    SET_DEVICES(state, devices) {
      state.devices = devices;
    },

    SET_DEVICECONFIG(state, config) {
      const index = state.devices.findIndex((c) => c.id === config.uuid);
      if (index != -1) {
        Vue.set(state.devices[index], "config", config.config);
        state.devices[index].config = config.config;
      }
    },

    SET_MEASUREMENTS(state, response) {
      const index = state.devices.findIndex((m) => m.id === response.uuid);

      if (index != -1) {
        Vue.set(state.devices[index], "measurements", [
          ...response.measurements,
        ]);
      }
    },

    SET_ACTIVE(state, uuid) {
      const index = state.devices.findIndex((d) => d.id === uuid);

      if (index != -1) {
        state.devices[index].activated = true;
      }
    },
    SET_NAME(state, device) {
      const index = state.devices.findIndex((d) => d.id === device.uuid);

      if (index != -1) {
        state.devices[index].name = device.name;
      }
    },
    DELETE_DEVICE(state, uuid) {
      const index = state.devices.findIndex((d) => d.id === uuid);
      if (index != -1) {
        state.devices.splice(index, 1);
      }
    },
  },
  actions: {
    loadDevices({ commit }) {
      axios.get(`${baseUrl}/api/devices`).then(({ data }) => {
        commit("SET_DEVICES", data);
      });
    },

    loadDeviceConfig({ commit }, uuid) {
      axios.get(`${baseUrl}/api/devices/${uuid}/config`).then(({ data }) => {
        commit("SET_DEVICECONFIG", { uuid: uuid, config: data });
      });
    },

    loadMeasurements({ commit, getters }, uuid) {
      axios
        .get(
          `${baseUrl}/api/devices/${uuid}/measurements?since=${moment()
            .subtract(7, "days")
            .format("YYYY.MM.DD")}`
        )
        .then(({ data }) => {
          commit("SET_MEASUREMENTS", { uuid: uuid, measurements: data });
        });
    },

    activateDevice({ commit }, uuid) {
      axios
        .patch(`${baseUrl}/api/devices/${uuid}`, { activated: true })
        .then(({ data }) => {
          commit("SET_ACTIVE", uuid);
        });
    },
    setDeviceName({ commit }, device) {
      axios
        .patch(`${baseUrl}/api/devices/${device.uuid}`, { name: device.name })
        .then(({ data }) => {
          commit("SET_NAME", device);
        });
    },
    deleteDevice({ commit }, uuid) {
      axios.delete(`${baseUrl}/api/devices/${uuid}`).then(() => {
        commit("DELETE_DEVICE", uuid);
      });
    },
    setDeviceConfig({ commit }, device) {
      axios
        .put(`${baseUrl}/api/devices/${device.uuid}/config`, device.config)
        .then(({ data }) => {
          commit("SET_DEVICECONFIG", {
            uuid: device.uuid,
            config: device.config,
          });
        });
    },
  },
});
