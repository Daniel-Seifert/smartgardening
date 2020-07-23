module.exports = {
  transpileDependencies: ["vuetify"],
  pwa: {
    name: "SmartGardening",
    themeColor: "#2E7D32",
    msTileColor: "#2E7D32",
    appleMobileWebAppCapable: "yes",
    appleMobileWebAppStatusBarStyle: "black",
    manifestOptions: {
      icons: [
        {
          src: "./img/icons/android-icon-192x192.png",
          sizes: "192x192",
          type: "image/png",
        },
        {
          src: "./img/icons/android-icon-512x512.png",
          sizes: "512x512",
          type: "image/png",
        },
      ],
    },
    iconPaths: {
      favicon32: "img/icons/favicon-32x32.png",
      favicon16: "img/icons/favicon-16x16.png",
      favicon96: "img/icons/favicon-96x96.png",
      appleTouchIcon: "img/icons/apple-icon.png",
      maskIcon: "img/icons/apple-icon.svg",
      msTileImage: "img/icons/ms-icon-144x144.png",
    },
  },
};
