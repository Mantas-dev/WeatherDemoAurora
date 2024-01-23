CREATE TABLE cities (
  city_name TEXT,
  latitude NUMERIC NOT NULL,
  longitude NUMERIC NOT NULL
);

CREATE TABLE favorites_cities (
  city_name TEXT,
  latitude NUMERIC NOT NULL,
  longitude NUMERIC NOT NULL
);

CREATE TABLE cached_cities (
  city_name TEXT PRIMARY KEY,
  latitude NUMERIC NOT NULL,
  longitude NUMERIC NOT NULL,
  last_weather_type TEXT,
  last_temperature TEXT,
  last_feels_like TEXT,
  last_humidity INTEGER
);

CREATE TABLE params (
  name TEXT PRIMARY KEY,
  value TEXT NOT NULL
);
