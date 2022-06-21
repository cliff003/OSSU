SELECT m.title
  FROM movies m
    LEFT JOIN ratings r
    ON m.id = r.movie_id
    LEFT JOIN stars s
    ON m.id = s.movie_id
    LEFT JOIN people p
    ON s.person_id = p.id
 WHERE p.name = "Chadwick Boseman"
ORDER BY r.rating DESC
 LIMIT 5;