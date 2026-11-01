# TODO

## Functionality

### Library ideas

- Multiple linear regression (see Machine Learning with R, p.184)
- from section 3.2:
  - if B is linear combination of A1...An, what are the multipliers?
  - span of matrices(A, B, ...)
  - is a matrix skew-symmetric?
  - is a matrix upper-triangular?
  - trace of a matrix
  - is A' the inverse of A?
  - is A invertible?
- from section 3.5:
  - is v in the row, col or null space of A?
  - calculate the basis or row, coll and null spaces
  - nullity(A)
  - do column vectors (n x n) form a basis for Rn?
  - is a vector in span and what are its coordinates in the span?
- from section 3.6:
  - rotate R2 vector theta degrees counter-clockwise
  - other transformations from the section


### la_calc application

- take libedit into use


## Exploration

- evaluate [raylib](https://www.raylib.com)
- use it to write a graphical application to visualise 2D vector operations
    * This is also for evaluating raylib for Dead Reckoning game, and a good
      and simple target before starting the real game development.


## Refactoring

### Separating core structures and algebra functions

Substantially done: `ref`/`rref`/`rank`, `det`, transforms, linear-system
solvers, and the vector algorithms (`dot`/`norm`/`angle`/`proj_onto`/...) all
live as free functions in dedicated headers
(`row_reduction.hpp`, `determinant.hpp`, `matrix_transforms.hpp`,
`matrix_linear_systems.hpp`, `vector_algorithms.hpp`).
Read Effective C++ for more.

Remaining:

- Decide whether `subvector` / `head` / `tail` count as "core" or
  belong with the algorithms (`include/la/vector.hpp:80-115`).
