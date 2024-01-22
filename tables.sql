CREATE TABLE favorites_cities (
  city_name TEXT PRIMARY KEY,
  latitude NUMERIC NOT NULL,
  longitude NUMERIC NOT NULL
);

CREATE TABLE cities (
  city_name TEXT PRIMARY KEY,
  latitude NUMERIC NOT NULL,
  longitude NUMERIC NOT NULL
);

CREATE TABLE params (
  name TEXT PRIMARY KEY,
  value TEXT NOT NULL
);
