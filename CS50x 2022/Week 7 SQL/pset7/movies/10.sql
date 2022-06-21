SELECT DISTINCT(p.name)
FROM people p
    LEFT JOIN directors d
    ON d.person_id = p.id
    LEFT JOIN movies m
    ON m.id = d.movie_id
    LEFT JOIN ratings r
    ON m.id = r.movie_id
WHERE r.rating >= 9.0;