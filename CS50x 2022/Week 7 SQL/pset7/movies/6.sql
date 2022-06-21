SELECT AVG(r.rating) AS avg_rating
FROM movies m
LEFT JOIN ratings r
ON m.id = r.movie_id
WHERE m.year = 2012;