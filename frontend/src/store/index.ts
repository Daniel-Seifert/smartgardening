import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";
import { Device } from "../model/Device";

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    devices: 
     [
      {
        uuid: "550e8400-e29b-11d4-a716-446655440000",
        name: "First Device",
        activated: true,
        createDate: new Date("02.02.2020"),
        updateDate: new Date("02.02.2020"),
        measurement: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
      },
      {
        uuid: "550e8400-e29b-11d4-a716-446655440001",
        name: "Second Device",
        activated: false,
        createDate: new Date("02.02.2020"),
        updateDate: new Date("02.02.2020"),
      },
      {
        uuid: "550e8400-e29b-11d4-a716-446655440002",
        name: "Third Device",
        activated: true,
        createDate: new Date("02.02.2020"),
        updateDate: new Date("02.02.2020"),
        measurement: [{id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:50"), value: 0},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:51"), value: 1},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:52"), value: 2},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:53"), value: 3},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:54"), value: 4},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:55"), value: 5},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:56"), value: 6},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:57"), value: 7},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:58"), value: 8},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 10:59"), value: 9},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 11:00"), value: 10},
         {id: 0, measureType: "asfdf",createDate: new Date("02.02.2020 11:01"), value: 11},
      ]

      }
    ] as Device[],
  },
  mutations: {
    SET_DEVICES(state, devices) {
      state.devices.push(devices);
    },

    SET_DEVICECONFIG(state, config) {
      const index = state.devices.findIndex(c => c.uuid === config.uuid);
      if (index != -1) {
        state.devices[index].config = config.config;
      }
    },

    SET_MEASUREMENT(state, measurements) {
      const index = state.devices.findIndex(
        m => m.uuid === measurements.uuid
      );

      if (index != -1) {
        state.devices[index].measurement = measurements;
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
      return state.devices.find(
        device => device.uuid === uuid
      )?.measurement?.map(i => i.value);
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
        commit("SET_DEVICECONFIG", data );
      });
    },

    loadMeasurements({ commit, getters }, uuid) {
      axios.get(uuid + "/measurements").then(({ data }) => {
        commit("SET_MEASUREMENT", data);
      });
    }
  }
});
