use std::io; // 표준 입출력 사용

fn main() {
    // 행렬 크기 입력
    let (rows, cols) = {
        let mut input = String::new();
        println!("행렬의 행(row) 수와 열(column) 수를 입력하세요 (예: 2 3):");
        io::stdin().read_line(&mut input).unwrap();
        let parts: Vec<usize> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().expect("숫자를 입력하세요"))
            .collect();
        if parts.len() != 2 {
            panic!("행(row)과 열(column) 두 값을 입력해야 합니다.");
        }
        (parts[0], parts[1])
    };

    // 두 행렬 입력
    println!("행렬 A의 값을 입력하세요:");
    let matrix_a = read_matrix(rows, cols);

    println!("행렬 B의 값을 입력하세요:");
    let matrix_b = read_matrix(rows, cols);

    // 두 행렬의 합 계산
    let result: Vec<Vec<i32>> = (0..rows)
        .map(|i| (0..cols).map(|j| matrix_a[i][j] + matrix_b[i][j]).collect())
        .collect();

    // 결과 출력
    println!("두 행렬의 합:");
    for row in result {
        println!("{}", row.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(" "));
    }
}

// 행렬 입력 함수
fn read_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut matrix = Vec::new();
    for i in 0..rows {
        let mut input = String::new();
        println!("{}번째 행의 {}개의 값을 공백으로 구분하여 입력하세요:", i + 1, cols);
        io::stdin().read_line(&mut input).unwrap();
        let row: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().expect("숫자를 입력하세요"))
            .collect();
        matrix.push(row);
    }
    matrix
}
