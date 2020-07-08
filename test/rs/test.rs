const SOME_CONST : i32 = 23;

fn main()
{
    let a = 32;
    let s = if a + SOME_CONST > 60 {
        a
    }
    else {
        SOME_CONST
    };

    print(s);
}