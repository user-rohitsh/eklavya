name := "akka"

version := "0.1"

scalaVersion := "2.12.6"

val workaround = {
  sys.props += "packaging.type" -> "jar"
  ()
}

libraryDependencies += "com.typesafe.akka" %% "akka-actor" % "2.5.14"
libraryDependencies += "com.typesafe.akka" %% "akka-stream" % "2.5.14"
libraryDependencies += "com.typesafe.akka" %% "akka-persistence" % "2.5.14"
libraryDependencies += "javax.ws.rs" % "javax.ws.rs-api" % "2.1"
libraryDependencies += "org.apache.kafka" %	"kafka-streams" %	"2.0.0"
libraryDependencies += "org.apache.kafka" % "kafka-clients" %	"2.0.0"
libraryDependencies += "org.apache.kafka" %%	"kafka-streams-scala" %	"2.0.0"