WITH t1 AS
(SELECT m.id
  FROM movies m
    LEFT JOIN stars s
    ON m.id = s.movie_id
    LEFT JOIN people p
    ON s.person_id = p.id
 WHERE p.name = "Kevin Bacon"
   AND p.birth = 1958)
SELECT DISTINCT(p.name)
  FROM movies m
    LEFT JOIN stars s
    ON m.id = s.movie_id
    LEFT JOIN people p
    ON s.person_id = p.id
 WHERE m.id in t1 AND p.name != "Kevin Bacon";