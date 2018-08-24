package futures_example
import scala.concurrent._


final case class Message(num: Int)

object futures_example {
  implicit val ec:ExecutionContext = ExecutionContext.global


  def apply() {

    val fut:Future[Int] = Future[Int] {
      print( "Thread Id = " + java.lang.Thread.currentThread().getId() + " value = 1")
      Thread.sleep(5000)
      1
    }

    fut.map( ( x:Int ) => {
      print( "Thread Id = " + java.lang.Thread.currentThread().getId() + " value = " + 50 +x )
      50 + x
    })

    print("created futures")
  }
}