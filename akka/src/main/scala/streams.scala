import akka.stream._
import akka.stream.scaladsl._


import akka.{NotUsed, Done}
import akka.actor.ActorSystem
import akka.util.ByteString
import scala.concurrent._
import scala.concurrent.duration._
import java.nio.file.Paths

final case class Message(num: Int)




object main extends App {
  val source: Source[Int, NotUsed] = Source(1 to 1000)
  val sink1: Sink[Int, Future[IOResult]] = Flow[Int]
    .map(n => n.toString())
    .map(s ⇒ ByteString(s + "\n")).
    toMat(FileIO.toPath(Paths.get("sink1.txt")))(Keep.right)

  val sink2: Sink[Int, Future[IOResult]] = Flow[Int]
    .map(n => n.toString())
    .map(s ⇒ ByteString(s + "\n")).
    toMat(FileIO.toPath(Paths.get("sink2.txt")))(Keep.right)

  implicit val system = ActorSystem("QuickStart")
  implicit val materializer = ActorMaterializer()

  val messages = for {n <- 1 to 1000
  } yield (Message(n))


  val message_source = Source(messages)

  val g = RunnableGraph.fromGraph(GraphDSL.create() {
    implicit builder => {
      import GraphDSL.Implicits._
      val bcast = builder.add(Broadcast[Message](2))
      message_source ~> bcast.in
      val f = Flow[Message].map(m => m.num)
      bcast.out(0) ~> f ~> sink1
      bcast.out(1) ~> f ~> sink2
      ClosedShape
    }
  }  )

  g.run()


}