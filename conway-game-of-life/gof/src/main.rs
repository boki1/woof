extern crate termion;

use std::io::{stdout, Write};
use std::thread;
use std::time::Duration;
use termion::{clear, color, cursor, raw::IntoRawMode};

const WIDTH: usize = 256;
const HEIGHT: usize = 256;

fn idx(x: usize, y: usize) -> usize {
    x * WIDTH + y
}

fn at(x: usize, y: usize, s: &'_ mut Vec<usize>, v_opt: Option<usize>) -> usize {
    if let Some(v) = v_opt {
        s[idx(x, y)] = v;
    }

    s[idx(x, y)]
}

fn neighbours(x: usize, y: usize, s: &'_ mut Vec<usize>) -> Vec<usize> {
    let mut neighbours = vec![];

    for xx in x - 1..x + 1 {
        for yy in y - 1..y + 1 {
            if xx == 0 && yy == 0 {
                continue;
            }
            neighbours.push(at(xx, yy, s, None));
        }
    }

    neighbours
}

fn main() {
    let mut screen = Vec::new();
    screen.resize(WIDTH * HEIGHT, 0);

    let mut stdout = stdout().into_raw_mode().unwrap();

    println!("{}", clear::All);

    loop {
        for x in 1..WIDTH + 1 {
            for y in 1..HEIGHT + 1 {
                let goto = cursor::Goto(x as u16, y as u16);
                let me = at(x, y, &mut screen, None);
                let all = neighbours(x, y, &mut screen);
                let active: usize = all.iter().sum();

                if active < 2 || active > 3 {
                    at(x, y, &mut screen, Some(0));
                }

                if me == 0 && active == 3 {
                    at(x, y, &mut screen, Some(1));
                }

                let me = at(x, y, &mut screen, None);

                print!("{}", goto);
                if me == 0 {
                    print!("{}", color::Fg(color::Black));
                } else {
                    print!("{}", color::Fg(color::White));
                }

                print!("#");
                thread::sleep(Duration::from_millis(500));
                stdout.flush().unwrap();
            }
        }
    }
}
