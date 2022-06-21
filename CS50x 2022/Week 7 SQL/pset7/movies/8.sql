SELECT p.name
FROM movies m
LEFT JOIN stars s
ON m.id = s.movie_id
LEFT JOIN people p
ON p.id = s.person_id
WHERE m.title = "Toy Story";