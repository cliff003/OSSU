WITH t1 AS
    (SELECT m.title, m.id
    FROM movies m
    LEFT JOIN stars s
    ON m.id = s.movie_id
    LEFT JOIN people p
    ON s.person_id = p.id
    WHERE p.name = "Johnny Depp"),
    t2 AS
    (SELECT m.title, m.id
    FROM movies m
    LEFT JOIN stars s
    ON m.id = s.movie_id
    LEFT JOIN people p
    ON s.person_id = p.id
    WHERE p.name = "Helena Bonham Carter")
SELECT t1.title
  FROM t1
  JOIN t2
 USING (id);