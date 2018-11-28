

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {
   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);


   public UrlValidatorTest(String testName) {
      super(testName);
   }


   public void testManualTest()
   {
   }

   public void printedAssertFalse(String url) {
      try {
         boolean result = urlVal.isValid(url);
         if (result) {
            System.out.println("TESTING " + url + " FAILED: expected FALSE but received TRUE");
         } else {
            System.out.println("TESTING " + url + " SUCCEEDED: received FALSE");
         }
      } catch(Error e) {
         System.out.println("TESTING " + url + " FAILED: received ERROR");
      }
   }

   public void printedAssertTrue(String message) {
      try {
         boolean result = urlVal.isValid(message);
         if (result) {
            System.out.println("TESTING " + message + " SUCCEEDED: received TRUE");
         } else {
            System.out.println("TESTING " + message + " FAILED: expected TRUE but received FALSE");
         }
      } catch(Error e) {
         System.out.println("TESTING " + message + " FAILED: received ERROR");
      }
   }

   public void testSchemePartition_Succeed()
   {
      System.out.println("\nScheme Partition - expect true");

      //You can use this function to implement your First Partition testing
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
      printedAssertTrue("http://www.google.com");
      printedAssertTrue("www.google.com");
      printedAssertTrue("ftp://www.google.com");
      printedAssertTrue("h3t://www.google.com");
   }

   public void testSchemePartition_Fail(){
		 //You can use this function to implement your Second Partition testing
      System.out.println("\nScheme Partition - expect false");

      printedAssertFalse("3ht://www.google.com");
      printedAssertFalse("http:/www.google.com");
      printedAssertFalse("http:www.google.com");
      printedAssertFalse("://www.google.com");
   }
   //You need to create more test cases for your Partitions if you need to

   public void testAuthorityPartition_Succeed()
   {
      System.out.println("\nAuthority Partition - expect true");

      //You can use this function to implement your First Partition testing
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
      printedAssertTrue("http://www.google.com");
      printedAssertTrue("http://go.com");
      printedAssertTrue("http://google.au");
      printedAssertTrue("http://0.0.0.0");
      printedAssertTrue("http://255.255.255.255");
   }

   public void testAuthorityPartition_Fail() {
      System.out.println("\nAuthority Partition - expect false");

      //You can use this function to implement your Second Partition testing
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

      printedAssertFalse("http://256.256.256.256");
      printedAssertFalse("http://255.com");
      printedAssertFalse("http://1.2.3.4.5");
      printedAssertFalse("http://go.a");
   }
   //You need to create more test cases for your Partitions if you need to

   public void testPartitionWithHTTP()
   {
      System.out.println("\nHTTP Partition - all URLS have http in them.");

      //You can use this function to implement your First Partition testing
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
      printedAssertTrue("http://www.google.com");
      printedAssertFalse("http:/go.com");
      printedAssertFalse("http:/");
      printedAssertFalse("http:/absdfs");
   }

   public void testPartitionWithoutHTTP(){

      System.out.println("\nHTTP Partition - no URLS have HTTP");

      //You can use this function to implement your Second Partition testing
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

      printedAssertTrue("ftp://www.google.com");
      printedAssertTrue("abc.com");
      printedAssertTrue("www.yahoo.edu");
   }

   public void testIsValid()
   {
	   //You can use this function for programming based testing

   }
   


}
