SELECT title, year AS release_year
FROM movies
WHERE title like "Harry Potter%"
ORDER BY year;