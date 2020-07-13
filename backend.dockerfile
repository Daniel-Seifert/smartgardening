# ---------------------
# Frontend Build
# ---------------------
FROM node:lts-alpine as BUILD-FRONTEND

WORKDIR /app

COPY ./frontend/package*.json ./
RUN npm install

COPY ./frontend ./
RUN npm run build

# ---------------------
# Backend Build
# ---------------------
FROM maven:3.6-jdk-13 as BUILD-BACKEND

WORKDIR /usr/app

COPY /backend/src /usr/app/src
COPY /backend/pom.xml /usr/app
COPY --from=BUILD-FRONTEND /app/dist /usr/app/src/main/resources/www/static

RUN mvn -f pom.xml -Djar.finalName=app -DskipTests clean package

# ---------------------
# Runtime Container
# ---------------------
FROM openjdk:13-jdk-alpine

ENV APP_HOME=/app
ENV STAGE="prod"
RUN adduser --system --no-create-home --disabled-password --ingroup root app
COPY --chown=app:root --from=BUILD-BACKEND /usr/app/target/*.jar $APP_HOME/app.jar

WORKDIR $APP_HOME
USER app
ENV MAX_MEM_PERCENT="80.0"

CMD java -XX:MaxRAMPercentage=${MAX_MEM_PERCENT} -Dspring.profiles.active=$STAGE -jar app.jar