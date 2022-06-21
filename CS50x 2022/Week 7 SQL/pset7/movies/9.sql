SELECT DISTINCT(p.name)
FROM movies m
    LEFT JOIN stars s
    ON m.id = s.movie_id
    LEFT JOIN people p
    ON p.id = s.person_id
WHERE m.year = 2004
ORDER BY p.birth;