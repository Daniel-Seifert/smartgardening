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
        activated: true,
      },
      {
        uuid: "550e8400-e29b-11d4-a716-446655440001",
        name: "Second Device",
        activated: false,
      },
      {
        uuid: "550e8400-e29b-11d4-a716-446655440002",
        name: "Third Device",
        activated: true,
      },
    ],
    measurementObjects: [
      {
        uuid: "550e8400-e29b-11d4-a716-446655440002",
        measurements: [1,2,3,4,5,6,7,8,10,11,12]
      }
    ],

  },
  mutations: {
    addData (state, payload) {
      const m = state.measurementObjects.find(m => m.uuid === payload.uuid)
      if (m === undefined){
        state.measurementObjects.push(payload)
      } else {
        m.measurements = payload.measurements
      }
    }

  },
  getters: {
    activatedDevices: state => {
      return state.devices.filter(device => device.activated)
    },
    allDevices: state => {
      return state.devices
    },
    measurement: (state) => (uuid) =>{
      return state.measurementObjects.find(measurement => measurement.uuid === uuid)?.measurements
    }
  },
  actions: {
    getMeassurements ({commit, getters}, uuid) {
      axios.get(uuid + "/measurements").then(({data}) => {
        commit('addData', {uuid: uuid, measurements:data})
      })
      return getters.measurement(uuid);
    }
  }
});
