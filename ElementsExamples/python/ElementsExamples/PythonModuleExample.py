import ElementsKernel.Logging as log

class ClassExample(object):
    """
    @class ClassExample
    @brief This is an example of the most basic Python class
    """                
    logger = log.getLogger('ClassExample')   
    def __init__(self, my_list):
        """
        @brief Constructor
        """                
        self._m_i = 5
        self._m_list = my_list
        
    def sumListValues(self):
        """
        @brief Method summing the elements of the list
        """                
        my_sum = 0
        for element in self._m_list:
            my_sum += element
        return my_sum
        
            
    @staticmethod
    def product(first, second):
        """
        @brief Procuct
        
        @details This is an example static method
        
        @param first
             The first number
        @param second
             The second number
        
        @returns
            The product of the two numbers
        """               
        return first * second 
