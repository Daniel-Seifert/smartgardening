import Vue from "vue";
import VueRouter, { RouteConfig } from "vue-router";
import Home from "../views/Home.vue";
import Settings from "../views/Settings.vue";

Vue.use(VueRouter);

const routes: Array<RouteConfig> = [
  {
    path: "/",
    name: "Home",
    component: Home,
  },
  {
    path: "/devices/:uuid/settings",
    name: "Settings",
    component: Settings,
  },
];

const router = new VueRouter({
  routes,
});

export default router;
