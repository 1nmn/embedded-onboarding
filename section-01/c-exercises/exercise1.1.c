/* How would you improve this code?

matrix_t matrix_transpose(matrix_t m) {
    matrix_t mt = create_matrix(m.cols, m.rows); //<- assume this has been implemented

    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            mt.data[j * m.rows + i] = m.data[i * m.cols + j];
        }
    }

    return mt;
}
*/

matrix_t matrix_transpose(matrix_t m)
{
    if (!m.data || m.rows <= 0 || m.cols <= 0) return; // Make sure input is valid

    matrix_t mt = create_matrix(m.cols, m.rows);

    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            if (i == j) continue; // Diagaonal elements remain the same
            mt.data[i * m.cols * j] = m.data[i * m.cols + j]
        }
    }

    return mt;
}
