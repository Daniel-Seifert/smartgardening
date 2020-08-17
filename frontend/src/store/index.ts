import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";
import { Device } from "../model/Device";
import { baseUrl } from "./baseUrl";
import moment from "moment";
import { Measurement } from "../model/Measurement";

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
    deviceStatus: (state) => (uuid: string) => {
      const status = state.devices.find((device) => device.id === uuid)?.status;
      return status
        ? status
        : { id: 0, pumping: false, humidity: 0, lastWatering: "n/a" };
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
              ...measurements
                .filter(
                  (measure) =>
                    measure.measureType === "MOISTURE" ||
                    (moment(measure.createDate).isAfter(moment.now()) &&
                      measure.measureType === "TEMP_DAY")
                )
                .map((measure) =>
                  moment(measure.createDate)
                    .lang("de")
                    .format("dd Do HH:mm")
                ),
            ],
            datasets: [
              {
                label: "Feuchtigkeit in %",
                backgroundColor: "#4fa1ff",
                data: [
                  ...measurements
                    .filter((measure) => measure.measureType === "MOISTURE")
                    .map((measure) => measure.value),
                ],
              },
              {
                label: "Regen in mm",
                backgroundColor: "#03d5ff",
                data: [
                  // Map rain to moisture values
                  ...measurements
                    .filter((measure) => measure.measureType === "MOISTURE")
                    .map((m) => {
                      const match = measurements
                        .filter((mes) => mes.measureType === "RAIN")
                        .find(
                          (it) =>
                            moment(it.createDate).day ===
                            moment(m.createDate).day
                        );
                      return match ? match.value : 0;
                    }),
                  // Add forecast
                  ...measurements
                    .filter(
                      (measure) =>
                        measure.measureType === "RAIN" &&
                        moment(measure.createDate).isAfter(moment.now())
                    )
                    .map((it) => it.value),
                ],
              },
              {
                label: "Bewölkung in %",
                backgroundColor: "#f5f5f5",
                data: [
                  ...measurements
                    .filter((measure) => measure.measureType === "MOISTURE")
                    .map((m) => {
                      const match = measurements
                        .filter((mes) => mes.measureType === "CLOUDS")
                        .find(
                          (it) =>
                            moment(it.createDate).day ===
                            moment(m.createDate).day
                        );
                      return match ? match.value : 0;
                    }),
                  // Add forecast
                  ...measurements
                    .filter(
                      (measure) =>
                        measure.measureType === "CLOUDS" &&
                        moment(measure.createDate).isAfter(moment.now())
                    )
                    .map((it) => it.value),
                ],
              },
              {
                label: "Temp in °C",
                backgroundColor: "#ffa166",
                data: [
                  ...measurements
                    .filter((measure) => measure.measureType === "MOISTURE")
                    .map((m) => {
                      const match = measurements
                        .filter((mes) => mes.measureType === "TEMP_DAY")
                        .find(
                          (it) =>
                            moment(it.createDate).day ===
                            moment(m.createDate).day
                        );
                      return match ? match.value : 0;
                    }),
                  // Add forecast
                  ...measurements
                    .filter(
                      (measure) =>
                        measure.measureType === "TEMP_DAY" &&
                        moment(measure.createDate).isAfter(moment.now())
                    )
                    .map((it) => it.value),
                ],
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
      }
    },
    SET_DEVICESTATUS(state, data) {
      data.status.lastWatering = moment(data.status.lastWatering).format(
        "DD.MM.YYYY HH:mm:ss"
      );
      const index = state.devices.findIndex((c) => c.id === data.uuid);
      if (index != -1) {
        Vue.set(state.devices[index], "status", data.status);
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
    loadStatus({ commit }, uuid) {
      axios.get(`${baseUrl}/api/devices/${uuid}/status`).then(({ data }) => {
        commit("SET_DEVICESTATUS", { uuid: uuid, status: data });
      });
    },
    loadMeasurements({ commit, getters }, uuid) {
      axios
        .get(
          `${baseUrl}/api/devices/${uuid}/measurements?since=${moment()
            .subtract(7, "days")
            .format("DD.MM.YYYY")}`
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
